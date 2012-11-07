using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Xml;

namespace PivotalTracker
{
    /// <summary>
    /// A Project on Pivotal.
    /// </summary>
    public class Project : Refreshable
    {
        /// <summary>
        /// All Stories of this Project.
        /// </summary>
        public List<Story> Stories { get; set; }

        /// <summary>
        /// Project's ID.
        /// </summary>
        public int ID { get; set; }

        public Project()
        {
            mode = RefreshMode.Automatic;
            Stories = new List<Story>();
            OnRefresh += OnRefreshProject;
        }

        public void OnRefreshProject(UserAuthentication authentication)
        {
            List<Story> currentStories = new List<Story>(Stories);
            List<Story> downloadedStories = GetStories(authentication);
            List<Story> selectedStories = new List<Story>();
            Console.WriteLine(downloadedStories[0].CreationDate);
            Console.Read();
            bool isNew = true;
            foreach (Story currentStory in currentStories)
            {
                foreach (Story downloadedStory in downloadedStories)
                {
                    if (currentStory.ID == downloadedStory.ID)
                    {
                        isNew = true;
                        if (mode == RefreshMode.Automatic)
                        {
                            if (DateTime.Compare(currentStory.LastUpdateDate, downloadedStory.LastUpdateDate) > 0) //The downloadedStory was last updated before the actualStory.
                            {
                                selectedStories.Add(new Story(currentStory));
                            }
                            else
                            {
                                selectedStories.Add(new Story(downloadedStory));
                            }
                            continue;
                        }
                        selectedStories.Add(new Story((mode == RefreshMode.ReplaceCurrentWithDownloaded) ? currentStory : downloadedStory));
                    }
                    if (isNew)
                    {
                        Stories.Add(new Story(downloadedStory));
                        isNew = true;
                    }
                }
            }
            UploadStories(authentication, selectedStories);
            currentStories.Clear();
            downloadedStories.Clear();
            selectedStories.Clear();
        }

        private void UploadStories(UserAuthentication authentication, List<Story> stories)
        {
            foreach (Story story in stories)
            {
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://www.pivotaltracker.com/services/v3/projects/" + ID.ToString() + "/stories/" + story.ID + "?token=" + authentication.Token);
                request.Method = "PUT";
                request.ContentType = "application/xml";
                string data = story.ToXmlString(authentication);
                byte[] datasAsBytes = Encoding.ASCII.GetBytes(data);
                request.ContentLength = datasAsBytes.Length;
                using (Stream stream = request.GetRequestStream())
                {

                    stream.Write(datasAsBytes, 0, datasAsBytes.Length);
                    stream.Close();
                }
            }
        }

        private List<Story> GetStories(UserAuthentication authentication)
        {
            return ConvertStringToStories(GetStoriesAsString(authentication));
        }

        private string GetStoriesAsString(UserAuthentication authentication)
        {
            HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create("http://www.pivotaltracker.com/services/v3/projects/" + ID.ToString() + "/stories?token=" + authentication.Token);
            using (StreamReader streamReader = new StreamReader(request.GetResponse().GetResponseStream()))
            {
                string streamLine = "";
                StringBuilder stringBuilder = new StringBuilder();
                while (streamLine != null)
                {
                    streamLine = streamReader.ReadLine();
                    if (!String.IsNullOrEmpty(streamLine))
                        stringBuilder.AppendLine(streamLine);
                }
                return stringBuilder.ToString();
            }
        }

        private List<Story> ConvertStringToStories(string stories)
        {
            List<Story> readyStories = new List<Story>();
            using (XmlReader reader = XmlReader.Create(new StringReader(stories)))
            {
                StringBuilder stringBuilder = new StringBuilder();
                string previousElement = "";
                string currentElement = "";
                Story currentStory = new Story();
                List<Task> readyTasks = new List<Task>();
                Task currentTask = new Task();
                List<StoryComment> readyComments = new List<StoryComment>();
                StoryComment currentComment = new StoryComment();
                List<StoryCommentAttachment> readyAttachments = new List<StoryCommentAttachment>();
                StoryCommentAttachment currentAttachment = new StoryCommentAttachment();
                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:
                            currentElement = reader.Name;
                            break;
                        case XmlNodeType.EndElement :
                            if (reader.Name == "story")
                            {
                                foreach (Task task in readyTasks)
                                {
                                    currentStory.Tasks.Add(new Task(task));
                                }
                                readyTasks.Clear();
                                foreach (StoryComment comment in readyComments)
                                {
                                    foreach (StoryCommentAttachment attachment in readyAttachments)
                                    {
                                        if ((comment.Author == attachment.Uploader) || (comment.Text == attachment.Description))
                                        {
                                            comment.Attachment = attachment;
                                        }
                                    }
                                    currentStory.Comments.Add(new StoryComment(comment));
                                }
                                readyAttachments.Clear();
                                readyComments.Clear();
                                readyStories.Add(new Story(currentStory));
                                currentStory = new Story();
                            }
                            else if (reader.Name == "task")
                            {
                                readyTasks.Add(new Task(currentTask));
                                currentTask = new Task();
                            }
                            else if (reader.Name == "note")
                            {
                                readyComments.Add(new StoryComment(currentComment));
                                currentComment = new StoryComment();
                            }
                            else if (reader.Name == "attachment")
                            {
                                readyAttachments.Add(new StoryCommentAttachment(currentAttachment));
                                currentAttachment = new StoryCommentAttachment();
                            }
                            break;
                        case XmlNodeType.Text:
                            switch (currentElement)
                            {
                                case "id" :
                                    if (previousElement == "" || previousElement == "story")
                                    {
                                        currentStory.ID = int.Parse(reader.Value);
                                    }
                                    else if (previousElement == "task")
                                    {
                                        currentTask.ID = int.Parse(reader.Value);
                                    }
                                    else if (previousElement == "note")
                                    {
                                        currentComment.ID = int.Parse(reader.Value);
                                    }
                                    else if (previousElement == "attachment")
                                    {
                                        currentAttachment.ID = int.Parse(reader.Value);
                                    }
                                    break;
                                case "story_type" :
                                    switch (reader.Value)
                                    {
                                        case "feature" :
                                            currentStory.Type = StoryType.Feature;
                                            break;
                                        case "chore" :
                                            currentStory.Type = StoryType.Chore;
                                            break;
                                        case "bug" :
                                            currentStory.Type = StoryType.Bug;
                                            break;
                                        case "release":
                                            currentStory.Type = StoryType.Release;
                                            break;
                                        default :
                                            break;
                                    }
                                    break;
                                case "url" :
                                    if (previousElement == "story")
                                    {
                                        currentStory.Url = reader.Value;
                                    }
                                    else if (previousElement == "attachment")
                                    {
                                        currentAttachment.Url = reader.Value;
                                    }
                                    break;
                                default :
                                    break;
                                case "estimate" :
                                    currentStory.Points = int.Parse(reader.Value);
                                    break;
                                case "current_state" :
                                    switch (reader.Value)
                                    {
                                        case "unstarted" :
                                            currentStory.State = StoryState.Unstarted;
                                            break;
                                        case "started" :
                                            currentStory.State = StoryState.Started;
                                            break;
                                        case "finished":
                                            currentStory.State = StoryState.Finished;
                                            break;
                                        case "delivered" :
                                            currentStory.State = StoryState.Delivered;
                                            break;
                                        case "accepted":
                                            currentStory.State = StoryState.Accepted;
                                            break;
                                        case "rejected":
                                            currentStory.State = StoryState.Rejected;
                                            break;
                                    }
                                    break;
                                case "description" :
                                    if (previousElement == "story")
                                    {
                                        currentStory.Description = reader.Value;
                                    }
                                    else if (previousElement == "task")
                                    {
                                        currentTask.Description = reader.Value;
                                    }
                                    else if (previousElement == "attachment")
                                    {
                                        currentAttachment.Description = reader.Value;
                                    }
                                    break;
                                case "name" :
                                    currentStory.Name = reader.Value;
                                    break;
                                case "requested_by" :
                                    currentStory.Requestor = reader.Value;
                                    break;
                                case "owned_by" :
                                    currentStory.Owner = reader.Value;
                                    break;
                                case "created_at" :
                                    if (previousElement == "story")
                                    {
                                        currentStory.CreationDate = DateTime.Parse(reader.Value.Replace("U", "").Replace("T", "").Replace("C", ""));
                                    }
                                    else if (previousElement == "task")
                                    {
                                        currentTask.CreationDate = DateTime.Parse(reader.Value.Replace("U", "").Replace("T", "").Replace("C", ""));
                                    }
                                    break;
                                case "updated_at" :
                                    break;
                                case "labels" :
                                    string currentLabel = "";
                                    for (int i = 0; i < reader.Value.Length; i++)
                                    {
                                        char character = reader.Value.ToCharArray()[i];
                                        if (character != ',')
                                        {
                                            currentLabel += character.ToString();
                                        }
                                        else
                                        {
                                            currentStory.Labels.Add(currentLabel);
                                            currentLabel = "";
                                        }
                                    }
                                    if (currentLabel != "")
                                        currentStory.Labels.Add(currentLabel);
                                    break;
                                case "position" :
                                    currentTask.Position = int.Parse(reader.Value);
                                    break;
                                case "complete" :
                                    currentTask.IsComplete = bool.Parse(reader.Value);
                                    break;
                                case "text" :
                                    currentComment.Text = reader.Value;
                                    break;
                                case "author" :
                                    if (previousElement == "note")
                                    {
                                        currentComment.Author = reader.Value;
                                    }
                                    break;
                                case "noted_at" :
                                    currentComment.CreationDate = DateTime.Parse(reader.Value.Replace("U", "").Replace("T", "").Replace("C", ""));
                                    break;
                                case "filename" :
                                    currentAttachment.FileName = reader.Value;
                                    break;
                                case "uploaded_by" :
                                    currentAttachment.Uploader = reader.Value;
                                    break;
                                case "uploaded_at" :
                                    currentAttachment.UploadDate = DateTime.Parse(reader.Value.Replace("U", "").Replace("T", "").Replace("C", ""));;
                                    break;
                            }
                            break;
                        default: 
                            break;
                    }
                    if (currentElement == "story" || currentElement == "task" || currentElement == "note" || currentElement == "attachment")
                        previousElement = currentElement;
                }
            }
            return readyStories;
        }
    }
}